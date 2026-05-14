from __future__ import annotations

import csv
from pathlib import Path

from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import cm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import Paragraph, SimpleDocTemplate, Spacer, Table, TableStyle


ROOT = Path(__file__).resolve().parent
FONT = "/System/Library/Fonts/Supplemental/Arial.ttf"


def register_fonts() -> str:
    pdfmetrics.registerFont(TTFont("Arial", FONT))
    return "Arial"


def load_preview_rows() -> list[list[str]]:
    path = ROOT / "quick_results.csv"
    if not path.exists():
        return [["quick_results.csv не найден", "", "", "", ""]]

    rows: list[list[str]] = [["dataset", "size", "algorithm", "time_us", "char_ops"]]
    with path.open(newline="", encoding="utf-8") as file:
        reader = csv.DictReader(file)
        for row in reader:
            if row["dataset"] == "random" and row["size"] in {"100", "400", "1000"}:
                rows.append([row["dataset"], row["size"], row["algorithm"], row["time_us"], row["char_ops"]])
            if len(rows) >= 19:
                break
    return rows


def build() -> None:
    font = register_fonts()
    styles = getSampleStyleSheet()
    styles.add(ParagraphStyle(name="RuTitle", fontName=font, fontSize=18, leading=22, spaceAfter=14))
    styles.add(ParagraphStyle(name="RuHeading", fontName=font, fontSize=14, leading=18, spaceBefore=12, spaceAfter=8))
    styles.add(ParagraphStyle(name="RuBody", fontName=font, fontSize=10.5, leading=14, spaceAfter=7))

    doc = SimpleDocTemplate(
        str(ROOT / "A1_report.pdf"),
        pagesize=A4,
        rightMargin=1.7 * cm,
        leftMargin=1.7 * cm,
        topMargin=1.5 * cm,
        bottomMargin=1.5 * cm,
    )

    story = [
        Paragraph("SET 9. Задача A1: анализ строковых сортировок", styles["RuTitle"]),
        Paragraph("Логинов Николай", styles["RuBody"]),
        Paragraph(
            "Цель работы — сравнить стандартные сортировки строк с алгоритмами, "
            "которые используют структуру строк и избегают лишних повторных сравнений общих префиксов.",
            styles["RuBody"],
        ),
        Paragraph("Подготовка данных", styles["RuHeading"]),
        Paragraph(
            "Класс StringGenerator генерирует строки длиной от 10 до 200 символов из алфавита "
            "A..Z, a..z, 0..9 и специальных символов !@#%:;^&*()-. Для эксперимента используются "
            "случайные, обратно отсортированные, почти отсортированные массивы и массивы с общим префиксом.",
            styles["RuBody"],
        ),
        Paragraph("Методика измерений", styles["RuHeading"]),
        Paragraph(
            "Класс StringSortTester измеряет среднее время работы и число посимвольных операций. "
            "Для стандартных Quick Sort и Merge Sort считаются сравнения символов внутри лексикографического "
            "сравнения. Для MSD Radix Sort учитываются обращения к символам, так как алгоритм распределяет "
            "строки по корзинам вместо прямого сравнения пар строк.",
            styles["RuBody"],
        ),
        Paragraph("Теоретическое сравнение", styles["RuHeading"]),
        Paragraph(
            "Обычные сортировки сравнением выполняют O(n log n) сравнений строк, но каждое сравнение может "
            "просматривать длинный общий префикс. String Quick Sort и MSD Radix Sort переходят к следующему "
            "символу только внутри групп с совпавшим префиксом, поэтому на строковых данных обычно уменьшают "
            "число повторных просмотров символов. Гибрид MSD Radix + String Quick Sort снижает накладные "
            "расходы на маленьких подмассивах.",
            styles["RuBody"],
        ),
        Paragraph("Фрагмент быстрых измерений", styles["RuHeading"]),
    ]

    table = Table(load_preview_rows(), repeatRows=1)
    table.setStyle(
        TableStyle(
            [
                ("FONTNAME", (0, 0), (-1, -1), font),
                ("FONTSIZE", (0, 0), (-1, -1), 8),
                ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#dfe8f5")),
                ("GRID", (0, 0), (-1, -1), 0.3, colors.grey),
                ("ALIGN", (1, 1), (-1, -1), "RIGHT"),
                ("VALIGN", (0, 0), (-1, -1), "MIDDLE"),
            ]
        )
    )
    story.extend(
        [
            table,
            Spacer(1, 0.4 * cm),
            Paragraph("Материалы для сдачи", styles["RuHeading"]),
            Paragraph(
                "Код эксперимента: A1_experiment.cpp. CSV быстрого прогона: quick_results.csv. "
                "ID посылок Codeforces: A1m — 374545278, A1q — 374545410, "
                "A1r — 374545486, A1rq — 374545543. "
                "Публичный репозиторий: https://github.com/elegant784seat/set9.git.",
                styles["RuBody"],
            ),
        ]
    )

    doc.build(story)


if __name__ == "__main__":
    build()
